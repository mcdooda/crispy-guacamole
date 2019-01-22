return {
  links = {
    {
      2,
      1,
      1,
      1,
    },
    {
      2,
      2,
      3,
      1,
    },
    {
      2,
      3,
      3,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
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
      name = 'components/behaviorcomponent',
    },
    {
      loadArguments = {
        1,
        {
          links = {
            {
              1,
              1,
              9,
              2,
            },
            {
              1,
              2,
              7,
              2,
            },
            {
              2,
              1,
              1,
              1,
            },
            {
              3,
              1,
              1,
              2,
            },
            {
              6,
              1,
              5,
              1,
            },
            {
              7,
              1,
              5,
              2,
            },
            {
              8,
              1,
              7,
              1,
            },
            {
              9,
              1,
              1,
              3,
            },
            {
              10,
              1,
              9,
              1,
            },
          },
          nodeType = 'statemachine',
          nodes = {
            {
              loadArguments = {
                3,
                1,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      2,
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
                      name = 'script/init',
                    },
                    {
                      name = 'print',
                    },
                    {
                      name = 'entity/gettemplatename',
                    },
                    {
                      name = 'script/getcontext',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'init',
              },
              name = 'string',
            },
            {
              name = 'statemachine/enter',
            },
            {
              loadArguments = {
                '',
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
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      2,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'print',
                    },
                    {
                      loadArguments = {
                        'foo',
                      },
                      name = 'string',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'foo',
              },
              name = 'string',
            },
            {
              loadArguments = {
                3,
                {
                  links = {
                    {
                      2,
                      1,
                      1,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      loadArguments = {
                        {
                          1,
                          '',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                    {
                      loadArguments = {
                        true,
                      },
                      name = 'boolean',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'true',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                4,
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
                'dummy loop',
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