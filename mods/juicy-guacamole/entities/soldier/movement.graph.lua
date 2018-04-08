script {
  links = {
    {
      1,
      1,
      6,
      1,
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
      4,
      1,
      1,
      3,
    },
    {
      5,
      1,
      1,
      4,
    },
  },
  nodes = {
    {
      name = 'components/movementcomponent',
    },
    {
      loadArguments = {
        2.5,
      },
      name = 'number',
    },
    {
      loadArguments = {
        2,
      },
      name = 'number',
    },
    {
      loadArguments = {
        3,
      },
      name = 'number',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
  },
}