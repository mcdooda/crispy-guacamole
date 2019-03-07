return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      3,
      1,
    },
    {
      1,
      3,
      3,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/playercontrollercomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        1,
        {
          links = {
            {
              1,
              1,
              10,
              2,
            },
            {
              2,
              2,
              6,
              2,
            },
            {
              3,
              1,
              2,
              1,
            },
            {
              5,
              1,
              4,
              1,
            },
            {
              6,
              1,
              4,
              2,
            },
            {
              7,
              1,
              6,
              1,
            },
            {
              8,
              1,
              2,
              2,
            },
            {
              9,
              1,
              8,
              1,
            },
            {
              10,
              2,
              8,
              2,
            },
            {
              11,
              1,
              10,
              1,
            },
          },
          nodeType = 'statemachine',
          nodes = {
            {
              name = 'statemachine/enter',
            },
            {
              loadArguments = {
                2,
                1,
                {
                  links = {
                    {
                      1,
                      1,
                      7,
                      1,
                    },
                    {
                      3,
                      1,
                      9,
                      1,
                    },
                    {
                      4,
                      1,
                      2,
                      2,
                    },
                    {
                      4,
                      1,
                      5,
                      1,
                    },
                    {
                      5,
                      1,
                      6,
                      1,
                    },
                    {
                      6,
                      1,
                      2,
                      3,
                    },
                    {
                      7,
                      1,
                      2,
                      1,
                    },
                    {
                      8,
                      1,
                      9,
                      2,
                    },
                    {
                      9,
                      1,
                      6,
                      2,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'entity/setelevation',
                    },
                    {
                      loadArguments = {
                        4,
                      },
                      name = 'number',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/getelevation',
                    },
                    {
                      name = 'math/add',
                    },
                    {
                      name = 'flow/eachframe',
                    },
                    {
                      name = 'game/gamedt',
                    },
                    {
                      name = 'math/multiply',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'move up',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                2,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      2,
                    },
                    {
                      1,
                      1,
                      6,
                      1,
                    },
                    {
                      3,
                      1,
                      7,
                      2,
                    },
                    {
                      4,
                      1,
                      5,
                      1,
                    },
                    {
                      5,
                      1,
                      2,
                      1,
                    },
                    {
                      6,
                      1,
                      7,
                      1,
                    },
                    {
                      7,
                      1,
                      2,
                      3,
                    },
                    {
                      8,
                      1,
                      3,
                      2,
                    },
                    {
                      9,
                      1,
                      3,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/setelevation',
                    },
                    {
                      name = 'math/multiply',
                    },
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'flow/eachframe',
                    },
                    {
                      name = 'entity/getelevation',
                    },
                    {
                      name = 'math/add',
                    },
                    {
                      name = 'game/gamedt',
                    },
                    {
                      loadArguments = {
                        -4,
                      },
                      name = 'number',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'move down',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                3,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      2,
                    },
                    {
                      2,
                      1,
                      5,
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      1,
                    },
                    {
                      4,
                      1,
                      3,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      loadArguments = {
                        'pi / 3',
                      },
                      name = 'math/formula',
                    },
                    {
                      name = 'math/greaterthan',
                    },
                    {
                      name = 'entity/getelevation',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      loadArguments = {
                        {
                          1,
                          'Result',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'high enough',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                6,
                {
                  links = {
                    {
                      1,
                      1,
                      4,
                      1,
                    },
                    {
                      2,
                      1,
                      1,
                      2,
                    },
                    {
                      3,
                      1,
                      1,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'entity/iscomponentenabled',
                    },
                    {
                      loadArguments = {
                        'projectile',
                      },
                      name = 'string',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      loadArguments = {
                        {
                          1,
                          'Is Enabled',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'projectile enabled',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                5,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'nope',
              },
              name = 'string',
            },
          },
        },
      },
      name = 'statemachine',
    },
  },
}